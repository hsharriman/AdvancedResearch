import cantools
import yaml
import subprocess
from pathlib import Path

NUM_MSGS = lambda msgs: len(msgs) - 1
RAW_ARR_NAME = lambda msg: msg.upper() + "_RAW_ARR"
STRUCT_NAME = lambda msg: msg.upper() + "_MSG"
TEMPLATE_SEPARATOR = "######\n"


class Utils:
    def __init__(self, file_name):
        self.name = file_name

    def edit_file(self, file_body):
        #open the file or create it if it doesn't exist already
        f = Path(self.name)
        f.touch(exist_ok=True) 
        with open(self.name, "r") as f:
            c_file = f.read()

        #update the file with the new body
        with open(self.name, "w") as f:
            f.write(file_body)
        print(f"updated {self.name}")

    def read_template(self, file):
        with open(file, "r") as f:
            return f.read()


class HeaderGenerator(Utils):
    def __init__(self, file_name):
        super().__init__(file_name=f"{file_name}_generated.h")
        self.prefix = file_name

    def generate(self, msgs):
        """
        Add board_CAN_init and initialize message_specific variables
        """
        header_str = self.read_template("./templates/h_file.txt").format(
            prefix=self.prefix,
            setters=self._define_setters(msgs)
        )
        return header_str

    def _define_setters(self, msgs):
        template = self.read_template("./templates/h_setters.txt")
        final_str = ""
        for msg, msg_info in msgs.items():
            if msg == "receivers":
                continue
            sig_str = ""
            for sig in msg_info["signals"].keys():
                final_str += template.format(msg=msg, sig=sig)
        return final_str


class CGenerator(Utils):
    def __init__(self, file_name):
        super().__init__(file_name=f"{file_name}_generated.c")
        self.prefix = file_name

    def _generate_can_arr(self, msgs):
        final_str = ""
        i = 0
        template = self.read_template("./templates/generate_can_arr.txt")
        for msg_name, msg_info in msgs.items():
            if msg_name == "receivers":
                continue
            #add naming to message name
            final_str += template.format(
                msg_name=msg_name.upper(),
                arr_name=RAW_ARR_NAME(msg_name),
                cycle_time=msg_info["frequency"],
                i=i,
                msg_id=msg_info["id"],
                msg_length=msg_info["data_bytes"]
            )
            i += 1
        return final_str

    def _generate_structs(self, msgs):
        template = self.read_template("./templates/generate_msg_structs.txt")
        def_temp, msg_temp, sig_temp = template.split(TEMPLATE_SEPARATOR)
        init_str, def_str = "", ""
        for msg, msg_info in msgs.items():
            if msg == "receivers":
                continue
            #create string that defines the structs, TODO make sure that struct type always matches type generated by cantools
            def_str += def_temp.format(struct_type=f"{self.prefix}_{msg}_t", struct_name=STRUCT_NAME(msg))
            
            #create string that initializes the structs
            sigs = msg_info["signals"]
            sig_str = ""
            for signal in sigs.keys():
                sig_str += sig_temp.format(signal=signal)
                #test that the naming scheme matches the cantools one
            init_str += msg_temp.format(struct_name=STRUCT_NAME(msg), signals=sig_str)
        return def_str, init_str

    def _generate_setters(self, msgs):
        final_str = ""
        template = self.read_template("./templates/c_setters.txt")
        for msg, msg_info in msgs.items():
            if msg == "receivers":
                continue
            sig_str = ""
            for sig in msg_info["signals"].keys():
                final_str += template.format(
                    msg=msg,
                    sig=sig,
                    prefix=self.prefix,
                    msg_raw=RAW_ARR_NAME(msg),
                    msg_struct=STRUCT_NAME(msg)
                )
        return final_str

    def _raw_array_defs(self, msgs):
        #initialize structs and raw arrays here, remove them from the header file
        header_str = ""
        template = self.read_template("./templates/raw_array_defs.txt")
        for msg_name in msgs.keys():
            if msg_name == "receivers":
                continue
            header_str += template.format(arr_name=RAW_ARR_NAME(msg_name))
        return header_str            

    def generate(self, msgs):
        #generate the entire can_msg_info array, leaving the raw_arr empty
        #use a list of all outgoing messages in PYTHON to determine this order.
        #generate a str array of the outgoing message names in C as well. Use this to fill in the RAW_ARR once each has been malloc'd..
        struct_definition, struct_init = self._generate_structs(msgs)
        func_body = self.read_template("./templates/c_file.txt").format(
            header=self.prefix, 
            can_arr=self._generate_can_arr(msgs),
            structs=struct_init, 
            struct_defs=struct_definition,
            NUM_MSGS=NUM_MSGS(msgs), 
            setters=self._generate_setters(msgs),
            raw_arrays=self._raw_array_defs(msgs)
        )
        return func_body

def main(dbc_file, yaml_file):
    """
    TODO
    """
    #load the cantools file
    db = cantools.database.load_file(f'../dbc/{dbc_file}.dbc')
    out = subprocess.run(['cantools', 'generate_c_source', '--no-floating-point-numbers', f'../dbc/{dbc_file}.dbc'])

    #open the yaml file
    #generate the add-on with the init function and the pre-defined pointers
    with open(f"../mini_yamls/{yaml_file}", 'r') as f:
        data = yaml.load(f, yaml.FullLoader)

    msgs = data["MessagesTX"]
    if NUM_MSGS(msgs) > 6:
        print("this mini YAML has more than the maximum 6 messages.")
        return

    yaml_file = yaml_file.strip(".yaml")
    c_gen = CGenerator(yaml_file)
    header = HeaderGenerator(yaml_file)
    #append the new additions to the header/C files that we just generated 

    init_func = c_gen.generate(msgs)
    c_gen.edit_file(init_func)

    header_body = header.generate(msgs)
    header.edit_file(header_body)


if __name__ == "__main__":
    main("test", "shutdown.yaml")   #these should eventually be named the same thing
