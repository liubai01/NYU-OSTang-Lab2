import unittest
import os
import logging
import filecmp
import subprocess
import time
import signal

from tests import tests

construct = False

with open("qa/Lab2QA.pytmplt") as f:
    addTestClass = f.read()

suite = unittest.TestSuite()

addTestSuite = """
suite.addTest(Test{name}{idx}("test"))
"""

for t in tests:
    exec(addTestClass.format(name=t["name"], idx=t["idx"], construct=construct))
    exec(addTestSuite.format(name=t["name"], idx=t["idx"]))


if __name__ == '__main__':
    # logging format
    logging.basicConfig(format='\033[1m\033[33m[Lab2 QA] %(levelname)s: \033[0m%(message)s', level=logging.DEBUG)

    os.chdir("qa")
    os.system("g++ ./tests/testDir/hello.cpp -o ./tests/testDir/hello")
    
    # unittest.main()
    runner = unittest.TextTestRunner(verbosity=0)
    runner.run(suite)

