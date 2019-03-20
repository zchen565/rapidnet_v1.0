import re
import pygraphviz as pgv


ruleDic = {'r2': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r1': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r13': lambda s1,s2 : 'know'+s1[4]+s2[5]}

def parseFile(file_name):
  with open(file_name, 'r') as f:
    s = f.readline().strip()
    s = preprocess(s)
    d = parse(s)
    return d


def preprocess(s):
    s = deleteRule(s, 'ra@n257')
    s = deleteRule(s, 'rb@n257')
    s = deleteRule(s, 'rc@n257')
    # s = deleteRule(s, 'rd@n257') 
    s = s.replace('knowEvent', 'know')
    s = s.replace('liveEvent', 'live')
    s = s.replace('likeEvent', 'like')
    s = s.replace('relation', 'know')
    return s

def firstLastMatch(s):
  count = 0
  for i in range(len(s)):
    if s[i]=='(':
      count += 1
    elif s[i]==')':
      count -= 1
      if count==0 and i<len(s)-1:
        return False
  return True

def checkParentheses(s):
  count = 0
  for c in s:
    if c=='(':
      count += 1
    elif c==')':
      count -= 1
  if count == 0:
    return True 
  else:
    return False 


def parse(s):
  if s.startswith('(') and firstLastMatch(s):
    return parse(s[1:-1])

  elif re.match(r'r.{1,2}@n257\(.+\)$', s):
    rl = []
    cl = []

    i = 0
    pre = 0
    count = 0
    while i<len(s):
      if s[i]=='(':
        count -= 1
      elif s[i]==')':
        count += 1
        if count==0:
          rule = s[pre:i+1]
          rl.append(rule)
          if i+1<len(s): 
            cl.append(s[i+1])
          else:
            break
          i += 1
          pre = i+1
      i += 1

    if len(rl)==1:
      dic = {}
      rule_name, rule_body = ruleParse(rl[0])
      dic[rule_name] = parse(rule_body)
      return dic
    elif '*' in cl:
      l = []
      for rule in rl:
        l.append(parse(rule))
      return l
    else:
      dic = {}
      for rule in rl:
        rule_name, rule_body = ruleParse(rule)
        dic[rule_name] = parse(rule_body)
      return dic

  else:
    l = []
    if '*' in s:
      i = s.index('*')
      l.append(s[:i])
      l.append(parse(s[i+1:]))
    else:
      l.append(s)
    return l
      


def ruleParse(rule):
  rule_name = rule.split('@')[0]
  pattern = re.search('r.{1,2}@n257\((.+)\)', rule)
  rule_body = pattern.group(1)
  return rule_name, rule_body


def setRuleNode(G, node_name):
  G.add_node(node_name)
  n = G.get_node(node_name)
  n.attr['shape'] = 'oval'
  n.attr['color'] = 'red'
  n.attr['fontsize'] = 10
  n.attr['fontname'] = 'sans-serif'
  n.attr['margin'] = 0.1


def setTupleNode(G, node_name):
  G.add_node(node_name)
  n = G.get_node(node_name)
  n.attr['shape'] = 'box'
  n.attr['fontsize'] = 10
  n.attr['fontname'] = 'sans-serif'
  n.attr['height'] = 0.3
  n.attr['width'] = 0.4
  n.attr['color'] = 'blue'


def giveNodeName(G, node_name, name_dic):
  if node_name not in G.nodes():
    name_dic[node_name] = 1
    return node_name
  else:
    name_dic[node_name] += 1
    return node_name+':'+str(name_dic[node_name]-1)

def drawGraphWithObj(G, obj, name_dic):
  l = []
  if isinstance(obj, dict):
    for key in obj.keys():
      node_name = giveNodeName(G, key, name_dic)
      setRuleNode(G, node_name)
      children = drawGraphWithObj(G, obj[key], name_dic)
      for child in children:
        G.add_edge(node_name, child)
      l.append(node_name)
    if len(l)>1:
      node_name = giveNodeName(G, 'add', name_dic)
      setTupleNode(G, node_name)
      for rule in l:
        G.add_edge(node_name, rule)
      return [node_name]

  elif isinstance(obj, list):
    for o in obj:
      children = drawGraphWithObj(G, o, name_dic)
      for child in children:
        l.append(child)
 
  else:
    node_name = giveNodeName(G, obj, name_dic)
    setTupleNode(G, node_name)
    l.append(node_name)

  return l


def drawGraph(file_name, save_name):
  d = parseFile(file_name)
  print d
  name_dic = {}
  G = pgv.AGraph(strict=True, directed=True)
  drawGraphWithObj(G, d, name_dic)
  G.layout()
  G.draw(save_name, prog='dot')


def deleteRule(s, rule):
  n = len(rule)
  ans = ''
  i = 0
  pre = 0
  while i<len(s)-n:
    if s[i:i+n]==rule:
      ans += s[pre:i]
      leftIndex = i+n
      rightIndex = leftIndex
      count = 0
      for j in range(leftIndex, len(s)):
        if s[j]=='(':
          count += 1
        elif s[j]==')':
          count -= 1
          if count==0:
            rightIndex = j
            break
      tmp = deleteRule(s[leftIndex+1:rightIndex], rule)
      ans += tmp
      pre = rightIndex+1
      i = rightIndex
    i += 1
  ans += s[pre:len(s)]
  return ans



def checkRedundancy(s):
  if s[0]=='(' and s[1]=='(' and \
    s[-1]==')' and s[-2]==')' and \
    firstLastMatch(s[1:-1]):
    return True
  return False

'''
def deleteRedundantBrackets(s):
  if not s.startswith('('):
    return s
  ans = ''
  i = 0
  count = 0
  pre = 0
  while i<len(s):
    while s[i]!='(':
      i += 1
    ans += s[pre:i]
    pre = i
    while i<len(s):
      if s[i]=='(':
        count += 1
      elif s[i]==')':
        count -= 1
        if count==0:
          tmp = s[pre:i+1]
          while checkRedundancy(tmp):
            tmp = deleteRedundantBrackets(tmp[1:-1])
          ans += tmp
          pre = i+1
      i += 1
    i += 1
  return ans
'''
          

if __name__ == '__main__':
  # d = parseFile('./data/prov/knows.txt')    
  # print d
  drawGraph('./data/prov/knows.txt', './data/plot/knows.png')
  '''
  G = pgv.AGraph(strict=True, directed=True)
  G.add_node(1)
  G.add_edge(2, 3)
  print G.string()
  G.layout()
  G.draw('data/plot/test.png')
  '''
