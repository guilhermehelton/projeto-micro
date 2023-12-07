import time
import math
import psutil
import asyncio
from websockets.server import serve

async def websocket_server(websocket, path):
    while True:
        message = display_usage()
        await websocket.send(message)
        await asyncio.sleep(1)

def display_usage():
    memory_usage = psutil.virtual_memory().percent
    cpu_usage_list = psutil.cpu_percent(percpu=True)
    cpu_usage = calc_media_cpu(cpu_usage_list)
    return f'{math.ceil(cpu_usage)};{math.ceil(memory_usage)}'

def calc_media_cpu(lista):
    n_cpu_usada = 0
    uso_total_medio = 0;
    for cpu in lista:
        if cpu != 0:
            uso_total_medio += cpu
            n_cpu_usada += 1
    return uso_total_medio / n_cpu_usada

async def main():
    async with serve(websocket_server, "Guilherme", 8765):
        await asyncio.Future()

if __name__ == '__main__':
    asyncio.run(main())
