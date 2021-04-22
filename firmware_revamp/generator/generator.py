import cantools
from pprint import pprint
import subprocess

class Message:
    def __init__(self, dbc_msg):
        self._parse_dbc(dbc_msg)
        self.frequency = 0  #????? TODO
    
    def _parse_dbc(self, dbc_msg):
        """
        BO_ 14 air_ctrl: 8 air_ctrl
            SG_ tsms_sense : 7|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ imd_sense : 15|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ main_pack__tsconn__sense : 23|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ connector_to_hvd__hvdconn__sense : 31|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ hvd_sense : 39|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ bms_sense : 47|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ imd_status : 55|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
            SG_ bms_status : 63|8@0+ (1,0) [0|1] "" bms_core,breaklight_bspd_shutdown,dashboard,throttle
        """
        for line in dbc_msg.split("\n"):
            if line.startswith("BO_"):
                for i, frag in enumerate(line.split(" ")):
                    if frag.endswith(":"):
                        self.msg_name = frag
                    if i == 1:
                        self.id = frag
                    if i== 3:
                        self.length = frag

def _generate_c_code():
    pass

def _generate_init_function():
    pass

if __name__ == "__main__":
    db = cantools.database.load_file('../dbc/test.dbc')
    out = subprocess.run(['cantools', 'generate_c_source', '--no-floating-point-numbers', '../dbc/test.dbc'])
    pprint(out)