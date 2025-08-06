import socket
import threading
import time

SERVER_HOST = 'localhost'
SERVER_PORT = 8080
NUM_CLIENTS = 4
MESSAGES_PER_CLIENT = 5

def client_worker(client_id):
    """Function run by each client thread"""
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((SERVER_HOST, SERVER_PORT))
        print(f"Client {client_id} connected")

        # Define messages
        messages = [
            f"SET client{client_id}_key1 value1",
            f"GET client{client_id}_key1",
            f"SET client{client_id}_counter 0",
            f"INCR client{client_id}_counter",
            f"GET client{client_id}_counter"
        ]

        # Send messages
        responses = []
        for msg in messages:
            try:
                sock.sendall((msg + "\n").encode('utf-8'))
                response = sock.recv(1024).decode('utf-8').strip()
                responses.append((msg, response))
                time.sleep(0.1)  # delay between messages
            except Exception as e:
                print(f"Client {client_id} error: {str(e)}")
                break

        # Print results for each client
        print(f"\nClient {client_id} results:")
        for msg, resp in responses:
            print(f"  {msg} : {resp}")

    except Exception as e:
        print(f"Client {client_id} failed to connect: {str(e)}")
    finally:
        sock.close()

def run_test():
    print(f"Starting test")

    # Create client threads
    threads = []
    for i in range(NUM_CLIENTS):
        t = threading.Thread(target=client_worker, args=(i,))
        threads.append(t)
        t.start()

    # Wait for threads to complete
    for t in threads:
        t.join()

    print(f"\nTest completed")

if __name__ == "__main__":
    run_test()