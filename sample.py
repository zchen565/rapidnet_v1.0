import pandas as pd
import numpy as np

def sample(data, sampled, iters, refer, num):
  tmp = pd.DataFrame(columns=data.columns)
  if iters==0:
    return tmp
  if refer<0:
    l = np.array(data.index)
    np.random.shuffle(l)
    for i in range(num):
      print l[i]
      print data.loc[l[i]:l[i]+1]
      print data
      tmp = tmp.append(data.loc[l[i]:l[i]+1])
      r = np.random.choice(tmp[['node1', 'node2']].values.reshape(-1))
      tmp = tmp.append(sample(data, tmp, iters-1, r, num))
  else:
    key = np.random.choice(['node1', 'node2'])
    ndata = data[data[key]==refer]
    l = np.array(ndata.index)
    np.random.shuffle(l)
    print ndata, l
    for i in range(min(num, len(l))):
      tmp = tmp.append(ndata.loc[l[i]:l[i]+1])
      r = np.random.choice(sampled[['node1', 'node2']].values.reshape(-1))
      tmp = tmp.append(sample(data, tmp, iters-1, r, num))
  return tmp


if __name__ == '__main__':
  data = pd.read_csv('./data/trust/bitcoinotc-prob.csv', sep=',')
  data = data[data['node1']<20][data['node2']<20]
  print data.shape
  sampled = pd.DataFrame(columns=data.columns)
  sampled = sample(data, sampled, 2, -1, 5)
  print sampled
  sampled.to_csv('./data/trust/sample.csv', encoding='gbk', index=False)
