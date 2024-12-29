from flask import Flask

app = Flask(__name__)

@app.route('/hello/<name>')
def hello_name(name):
   return 'Hello %s!' % name

def runServer():
   app.run(debug=False)