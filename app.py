# app.py
from flask import Flask, request, jsonify
import joblib

# تحميل الموديل
model = joblib.load("baby_model.pkl")

app = Flask(__name__)

@app.route('/predict', methods=['POST'])
def predict():
    data = request.json
    temp = data.get('temp')
    cry = data.get('cry')
    move = data.get('move')

    # معالجة البيانات
    input_data = [[temp, cry, move]]
    prediction = model.predict(input_data)[0]

    return jsonify({'result': prediction})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
