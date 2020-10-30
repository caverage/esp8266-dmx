""" Get DMX channel values and send to DMX controller

Args:
    1: dmx controller host (IP or fqdn)
    2: dmx controller port
    3: dmx channel quantity

"""

import socket
import sys


def main(host: str, port: int, dmx_channel_qty: int) -> None:
    if dmx_channel_qty > 512:
        raise RuntimeError("maximum of 512 DMX channels")

    dmx_channels = bytearray()

    for channel in range(1, dmx_channel_qty + 1):
        user_input = input(f"Channel {channel}: ")
        if int(user_input) > 255 or int(user_input) < 0:
            raise RuntimeError("Channel value must be 0-255 inclusive")

        dmx_channels.append(int(user_input))

    controller_connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    controller_connection.connect((host, port))

    print(dmx_channels)

    controller_connection.sendall(dmx_channels)


if __name__ == "__main__":
    main(
        host=sys.argv[1], port=int(sys.argv[2]), dmx_channel_qty=int(sys.argv[3]),
    )
