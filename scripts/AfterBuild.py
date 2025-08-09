import socket

maya_port = 20241 

command = (
    'cmds.file("D:/study/Maya-Api-Learning-Journey/test-scene/Test-Scene.mb", open=1, force=1)\n'
    'cmds.loadPlugin("MayaApiLearningJourney")\n'
    'cmds.select("pCube1")\n'
)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(('localhost', maya_port))
    s.sendall(command.encode('utf-8'))
