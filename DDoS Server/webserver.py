from flask import Flask
from CC_server import main, getAttackDetails
import threading

app = Flask(__name__)

@app.route('/hello/<name>')
def hello_name(name):
   return 'Hello %s!' % name

@app.route('/attack_details', methods=['GET'])
def attack_details():
   return getAttackDetails()

def runCCServer():
   CC_thread = threading.Thread(target=main, daemon=True)
   CC_thread.start()


def runServer():
   runCCServer()
   app.run(debug=False, use_reloader=False)

if __name__ == "__main__":
    try:
        runServer()
    except KeyboardInterrupt:
        print("Shutting down...")
        exit(0)