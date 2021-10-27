from flask import Flask, request, make_response, Response
from plivo import plivoxml

app = Flask(__name__)

@app.route('/forward_call/', methods=['GET', 'POST'])
def forwardcall():
   print("test")
   response = plivoxml.ResponseElement()
   response.add(
    plivoxml.SpeakElement('Your call is being connected'))
   response.add(
       plivoxml.DialElement().add_number('8042221111')) #call will be forwarded to this number
   return Response(response.to_string(), mimetype="text/xml")

if __name__ == '__main__':
   app.run(host='0.0.0.0', debug=True)