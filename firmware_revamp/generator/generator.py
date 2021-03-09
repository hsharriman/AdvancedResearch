import cantools
from pprint import pprint
import subprocess

if __name__ == "__main__":
    db = cantools.database.load_file('../dbc/test.dbc')
    out = subprocess.run(['cantools', 'generate_c_source', '--no-floating-point-numbers', '../dbc/test.dbc'])
    pprint(out)