import socket

maya_port = 20241

command = (
    'cmds.file(new=True, force=True)\n'
    'cmds.unloadPlugin("MayaApiLearningJourney")\n'
)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(('localhost', maya_port))
    s.sendall(command.encode('utf-8'))