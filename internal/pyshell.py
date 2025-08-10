import code
import sys
import platform

# get the MSong API
from MSong import SongAPI

musicapi = SongAPI()
py_ver = sys.version

class MSongPy(code.interactiveConsole():
	def __init__(self, locals=None):
		super().__init__(locals)
		print(f"MSong Python {py_version}")
		print(platform.python_version())

if __name__ == "__main__":
	console = MSongPy(locals=none)
	console.interact()
