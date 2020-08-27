#!/usr/bin/env python3

import math


class TooManyCallsError(Exception):
    '''Trida TooManyCallsError'''
    def __init__(self, message):
        '''init'''
        super().__init__(message)

call_num=0
def limit_calls(max_calls=2, error_message_tail="called too often"):
    '''limit_calls'''

    def decorator(func):
        def wrapper(*args):
            global call_num
            call_num+=1
            
            name=func.__name__
            if (call_num > max_calls):
                raise TooManyCallsError("function \""+name+"\" - "+error_message_tail)

            return func(*args)

        return wrapper
    return decorator

@limit_calls(1, 'that is too much')
def pyth(a,b):
    '''pyth'''
    c = math.sqrt(a**2 + b ** 2)
    return c


def ordered_merge(*args,**selector):
    """Funkce vrati na zaklade selektoru prvky v zadanem poradi"""
    if( len(selector)==0 or len(list(selector.values())[0])==0):
        return []
    else:
        lst = []
        poz=list(selector.values())[0]

        for idx in range(len(args)):
            lst.append(iter(args[idx]))

        for j in poz:
            yield (next(lst[j]))


class Log:
    '''Trida Log'''

    def __init__(self, path):
        '''init'''
        self.path=path

    def __enter__(self):
        '''enter'''
        self.f=open(self.path,"a+")
        self.f.write("Begin\n")
        return self

    def __exit__(self, exception_type, exception_value, traceback):
        '''exit'''
        self.f.write("End\n")
        self.f.close()
        return None

    def logging(self,txt):
        '''logging'''
        self.f.write(txt+"\n")

