#!/usr/bin/env python3


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


df = pd.read_csv('cmake-build-debug/exp.csv')
df.sort_values(by=['logT', 'cnt'], inplace=True)
df = df.groupby(by=['logT', 'cnt']).agg({'hybrid_time': 'mean', 'bucket_time': 'mean', 'rotem_time': 'mean', 'subset_time': 'mean'}).reset_index()
for logT in df['logT'].unique().tolist():
    df_t = df[(df['logT'] == logT) & (df['cnt'] <= 1000)]
    show_x_y(df_t, 'cnt', ['hybrid_time', 'bucket_time', 'rotem_time', 'subset_time'],
         ['Time of hybrid approach', 'Time of bucket approach', 'Time of Rotem approach', 'Time of subset approach'], 'instances to batch', 'running time, seconds', f'Comparison for logT = {logT}, up to 1000 instances')
    df_t = df[(df['logT'] == logT) & (df['cnt'] >= 1000)]
    show_x_y(df_t, 'cnt', ['hybrid_time', 'bucket_time', 'rotem_time', 'subset_time'],
         ['Time of hybrid approach', 'Time of bucket approach', 'Time of Rotem approach', 'Time of subset approach'], 'instances to batch', 'running time, seconds', f'Comparison for logT = {logT}, 10^3 - 10^5 instances')
