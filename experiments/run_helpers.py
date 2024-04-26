import subprocess

import plotly.graph_objs as go
import pandas as pd


def show_x_y(df, x_name, y_names, _names, x_q, y_q, _title):
    traces = []
    i = 0
    for y_name in y_names:
        trace = go.Scatter(x=df[x_name], y=df[y_name], mode='lines+markers', name=_names[i])
        traces.append(trace)
        i += 1
    layout = go.Layout(yaxis=dict(title=y_q), xaxis=dict(title=x_q), title=_title)
    fig = go.Figure(data=traces, layout=layout)
    fig.show(renderer='browser')


def basic_run(run_args):
    cmd = run_args['build_path'] + ' ' + run_args['protocol_type'] + ' ' + str(run_args['logt']) + ' '
    cmd += str(run_args['experiments']) + ' ' + str(run_args['instances_per_exp']) + ' ' + run_args['output_file']
    try:
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   universal_newlines=True)
        for line in process.stdout:
            print(line, end='')
        for line in process.stderr:
            print(line, end='')
        process.wait()
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {cmd}")
        print(e)
        return
    else:
        print(f"Command executed successfully: {cmd}")




def all_but_naive_run(run_args):
    run_args['protocol_type'] = 'all_but_naive'
    basic_run(run_args)

def subset_run(run_args):
    run_args['protocol_type'] = 'subset'
    basic_run(run_args)

def exponent_run(run_args):
    run_args['protocol_type'] = 'exponential'
    basic_run(run_args)

def bucket_run(run_args):
    run_args['protocol_type'] = 'bucket'
    basic_run(run_args)

def hybrid_run(run_args):
    run_args['protocol_type'] = 'hybrid'
    basic_run(run_args)

