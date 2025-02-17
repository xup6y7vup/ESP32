from flask import Flask, request
import time

app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload_image():
    if 'image' not in request.files:
        return "No image file", 400

    image = request.files['image']
    filename = f"captured_{int(time.time())}.jpg"  # Save file with timestamp
    image.save(filename)

    print(f"Image saved as {filename}")
    return "Image received", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)  # Run the server on your local network
