#!/usr/bin/python3
# Image Recognition Using Tensorflow Exmaple.
# Code based on example at:
# https://raw.githubusercontent.com/tensorflow/tensorflow/master/tensorflow/examples/label_image/label_image.py
import os
#os.environ['TF_CPP_MIN_LOG_LEVEL'] = '1'
import tensorflow as tf
#tf.logging.set_verbosity(tf.logging.ERROR)
import numpy as np
import threading
import queue
import time
import sys
from shutil import copyfile
from multiprocessing.dummy import Pool as ThreadPool

config = tf.ConfigProto()
config.gpu_options.allow_growth = True
tf.config.threading.set_inter_op_parallelism_threads(2)
tf.config.threading.set_intra_op_parallelism_threads(6)


config.gpu_options.per_process_gpu_memory_fraction = 1


import base64
import requests
import json
import time
import sys



def convert_and_save_image(base46_data,id):
    imgdata = base64.b64decode(base46_data)
    filename = './unknown_images/'+str(id)+'.png'
    with open(filename, 'wb') as f:
        f.write(imgdata)


# sudo apt install python3-pip
# sudo python3 -m pip install --upgrade pip
# sudo python3 -m pip install --upgrade setuptools
# sudo python3 -m pip install --upgrade tensorflow==1.15

def load_labels(label_file):
    label = []
    proto_as_ascii_lines = tf.gfile.GFile(label_file).readlines()
    for l in proto_as_ascii_lines:
        label.append(l.rstrip())
    return label

def predict_image(q, sess, graph, image_bytes, img_full_path, labels, input_operation, output_operation):
    image = read_tensor_from_image_bytes(image_bytes)
    results = sess.run(output_operation.outputs[0], {
        input_operation.outputs[0]: image
    })
    results = np.squeeze(results)
    prediction = results.argsort()[-5:][::-1][0]
    q.put( {'img_full_path':img_full_path, 'prediction':labels[prediction].title(), 'percent':results[prediction]} )

def predict_image_second(sess, image_bytes, input_operation, output_operation):
    image = read_tensor_from_image_bytes(image_bytes)
    results = sess.run(output_operation.outputs[0], {
        input_operation.outputs[0]: image
    })
    results = np.squeeze(results)
    prediction = results.argsort()[-5:][::-1][0]
    return prediction

def load_graph(model_file):
    graph = tf.Graph()
    graph_def = tf.GraphDef()
    with open(model_file, "rb") as f:
        graph_def.ParseFromString(f.read())
    with graph.as_default():
        tf.import_graph_def(graph_def)
    return graph

def read_tensor_from_image_bytes(imagebytes, input_height=299, input_width=299, input_mean=0, input_std=255):#255
    image_reader = tf.image.decode_png( imagebytes,channels=3,name="png_reader")
    float_caster = tf.cast(image_reader, tf.float64)
    dims_expander = tf.expand_dims(float_caster, 0)
    resized = tf.image.resize_bilinear(dims_expander, [input_height, input_width])
    normalized = tf.divide(tf.subtract(resized, [input_mean]), [input_std])
    #sess = tf.compat.v1.Session() 
    sess = tf.Session(config=config)

    result = sess.run(normalized)
    return result




if 1==1:
    # Loading the Trained Machine Learning Model created from running retrain.py on the training_images directory
    graph = load_graph('/tmp/retrain_tmp/output_graph.pb')
    labels = load_labels("/tmp/retrain_tmp/output_labels.txt")

    # Load up our session
    input_operation = graph.get_operation_by_name("import/Placeholder")
    output_operation = graph.get_operation_by_name("import/final_result")
    sess = tf.compat.v1.Session(graph=graph)

    # Can use queues and threading to spead up the processing
    unknown_images_dir = 'unknown_images'


    START = time.time()

    response = requests.post("https://fridosleigh.com/api/capteha/request",verify=True)
    response_js = json.loads(response.text)
    requested_elements = response_js['select_type']
    session= (response.cookies.get_dict()['session'])

    END = time.time()
    print ("----------------------------------------------------------------------------")
    print (END-START)
    print ("----------------------------------------------------------------------------")
    START = time.time()
    result = []
    for image in response_js['images']:
        #convert_and_save_image(image['base64'],image['uuid'])
        image_bytes = base64.standard_b64decode(image['base64'])
        value = predict_image_second(sess, image_bytes, input_operation, output_operation)
        current_result = {}
        current_result['id'] = image['uuid']
        current_result['result'] = labels[value]
        result.append(current_result)
        #print (current_result)


    END = time.time()
    print ("----------------------------------------------------------------------------")
    print (END-START)
    print (len(result))
    print ("----------------------------------------------------------------------------")
    answer=[]
    START = time.time()
    print (requested_elements)
    for requested in requested_elements.split(','):
        for prediction in result:
            if prediction['result'].lower() in requested.lower():
                #print (prediction)
                answer.append(str(prediction['id']))


    #print (answer)
    #print len(answer)
    cookies = {'session': session}
    response = requests.post('https://fridosleigh.com/api/capteha/submit',data={'answer':",".join(answer),'email':'walid.daboubi@richemont.com','name':'Sunhak','age':'189','about':'best sunhak','favorites':'sugarcookiesantas'},cookies=cookies,verify=True)
    END = time.time()
    #print response.content
    #print ("----------------------------------------------------------------------------")
    #print (END-START)
    #print ("----------------------------------------------------------------------------")
    #print (json.loads(response.text))
    print response.request.body
    print "*************"
    print response.headers
    print response.cookies.get_dict()
    print response.content
    print "//////////////////////////////"
    session= (response.cookies.get_dict()['session'])
    cookies = {'session': session}
    while 1==1:
        response = requests.post('https://fridosleigh.com/api/entry',data={'email':'walid.daboubi@gmail.com','name':'sunhak','age':'189','about':'best sunhak','favorite':'.'.join(['sugarcookiesantas'])},cookies=cookies,verify=True)

        print response.headers
        print response.cookies.get_dict()
        print response.content
        session= (response.cookies.get_dict()['session'])
        cookies = {'session': session}
        print "######################################################"
#if __name__ == "__main__":
#    main()
