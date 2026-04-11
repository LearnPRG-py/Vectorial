import subprocess
import sys
from pathlib import Path

build_dir = Path("build")
subprocess.run(["cmake", "-B", str(build_dir)], check=True)
subprocess.run(["cmake", "--build", str(build_dir)], check=True)

test_bin = build_dir / ("test.exe" if sys.platform == "win32" else "test")
subprocess.run([str(test_bin)], check=True)
