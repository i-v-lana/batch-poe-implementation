#!/usr/bin/env python3


import plotly.graph_objs as go
import pandas as pd


def show_x_y(df, x_name, y_names, _names, y_q, _title):
    traces = []
    i = 0
    for y_name in y_names:
        trace = go.Scatter(x=df[x_name], y=df[y_name], mode='lines', name=_names[i])
        traces.append(trace)
        i += 1
    layout = go.Layout(yaxis=dict(title=y_q), title=_title)
    fig = go.Figure(data=traces, layout=layout)    
    fig.show(renderer='browser')


df = pd.read_csv('exp_results.csv')
df.sort_values(by=['logT', 'k'], inplace=True)
df = df.groupby(by=['logT', 'k']).agg({'time_hybrid': 'mean', 'time_bucket': 'mean'}).reset_index()
for logT in df['logT'].unique().tolist():
    df_t = df[df['logT'] == logT]
    show_x_y(df_t, 'k', ['time_hybrid', 'time_bucket'],
         ['Time of hybrid approach', 'Time of bucket approach'], 's', f'Comparison for logT = {logT}')
