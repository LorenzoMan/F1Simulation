from telemetry import *
import socket
import socketio
import ctypes
import eventlet

thread = None

sio = socketio.Server(cors_allowed_origins='*')
app = socketio.WSGIApp(sio, static_files={
    '/' : './public/'
})

@sio.event
def getdata():
    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.bind(("IP Adress from the computer you are running the code from", "Port"))
    while True:
        data, _ = sock.recvfrom(1341)
        header = Header.from_buffer_copy(data[0:21])
        if int(header.m_packetId) == 0:
            packet = PacketMotionData.from_buffer_copy(data[0:1341])
            sio.emit("Lat", round(packet.cars_motion_data[header.m_playerCarIndex].m_gForceLateral, 3))
            sio.emit("Lon", round(packet.cars_motion_data[header.m_playerCarIndex].m_gForceLongitudinal, 3))

        elif int(header.m_packetId) == 1:
            packet = PacketSessionData.from_buffer_copy(data[0:147])

        elif int(header.m_packetId) == 2:
            packet = PacketLapData.from_buffer_copy(data[0:841])

        elif int(header.m_packetId) == 3:
            packet = PacketEventData.from_buffer_copy(data[0:25])

        elif int(header.m_packetId) == 4:
            packet = PacketParticipantsData.from_buffer_copy(data[0:1082])

        elif int(header.m_packetId) == 5:
            packet = PacketCarSetupData.from_buffer_copy(data[0:841])

        elif int(header.m_packetId) == 6:
            packet = PacketCarTelemetryData.from_buffer_copy(data[0:1085])

        elif int(header.m_packetId) == 7:
            packet = PacketCarStatusData.from_buffer_copy(data[0:1061])
        sio.sleep(0.0001)

@sio.event
def connect(sid, data):
    global thread
    if thread is None:
        thread = sio.start_background_task(getdata)

eventlet.wsgi.server(eventlet.listen(('', 8080)), app)
