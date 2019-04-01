import re
import pygraphviz as pgv



def trustR2(s1, s2):
  s1 = s1[5:] if s1.startswith('trust') else s1[10:]
  s2 = s2[5:] if s2.startswith('trust') else s2[10:]
  a1, a2 = s1.split('-')
  a3, a4 = s2.split('-')
  if a2==a3:
    return 'trustPath'+a1+'-'+a4
  else:
    return 'trustPath'+a3+'-'+a2

ruleDic = {'r2': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r1': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r13': lambda s1,s2 : 'know'+s1[4]+s2[5] if s1[5]==s2[4] else 'know'+s2[4]+s1[5],
           'rd': lambda s : 'know'+s[8]+s[9],
           'r0': lambda s : s}

trustDic = {'ra': lambda s: 'trustEvent'+s[10:],
            'r1': lambda s1,s2: 'trustPath'+s[5:],
            'r2': trustR2,
            'r3': lambda s1,s2: 'mutualTrustPath'+s1[10:]}

nameDic = {0: 'ben', 1: 'Elena', 2: 'Dhanya', 3: 'Alex', 4:'Arti',
           5: 'Sabina', 6: 'Jay', 7: 'Steve'}

def parseFile(file_name):
  with open(file_name, 'r') as f:
    s = f.readline().strip()
    s = preprocess(s)
    print s
    d = parse(s)
    return d


def preprocess(s):
    s = deleteRule(s, 'ra@n257')
    s = deleteRule(s, 'rb@n257')
    s = deleteRule(s, 'rc@n257')
    s = deleteRule(s, 'rd@n257') 
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

def deleteParentheses(s):
  while s.startswith('(') and firstLastMatch(s):
    s = s[1:-1]
  return s

def parse(s):
  if s.startswith('(') and firstLastMatch(s):
    return parse(s[1:-1])

  rl, cl = splitRule(s)

  if '*' in cl:
    l = []
    for rule in rl:
      rule = deleteParentheses(rule)
      if not re.match(r'r.{1,2}@n257\(.+\)$', rule) and len(splitRule(rule)[0])==1:
        l.append(rule)
      else:
        l.append(parse(rule))
    return l
  else:
    dic = {}
    for rule in rl:
      rule = deleteParentheses(rule)
      rule_name, rule_body = ruleParse(rule)
      if rule_name!='r0':
          dic[rule_name] = parse(rule_body)
      else:
        if '*' not in rule:
          dic[rule_name] = rule_body
        else:
          dic[rule_name] = parse(rule_body)
    return dic


def splitRule(s):
  rl = []
  cl = []
  i = 0
  pre = 0
  count = 0
  while i<len(s):
    if s[i]=='*' or s[i]=='+':
      if count==0:
        cl.append(s[i])
        rl.append(s[pre:i])
        pre = i+1
    if s[i]=='(':
      count += 1
    elif s[i]==')':
      count -= 1
    i += 1
  rl.append(s[pre:i])
  return rl, cl

def ruleParse(rule):
  if re.match(r'r.{1,2}@n257\(.+\)$', rule):
    print rule
    rule_name = rule.split('@')[0]
    pattern = re.search('r.{1,2}@n257\((.+)\)', rule)
    rule_body = pattern.group(1)
  else:
    rule_name = 'r0'
    rule_body = rule
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
    query_name = None
    for key in obj.keys():
      node_name = giveNodeName(G, key, name_dic)
      setRuleNode(G, node_name)
      children = drawGraphWithObj(G, obj[key], name_dic)
      for child in children:
        G.add_edge(node_name, child)
      if key!='r0':
        query_name = ruleDic[key](children[0], children[1])
        query_name = giveNodeName(G, query_name, name_dic)
      else:
        query_name = ruleDic[key](children[0])
      l.append(node_name)
    node_name = giveNodeName(G, query_name, name_dic)
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
    if obj[:4]=='know':
      obj = obj[:-1]
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
