# -*- coding: utf8 -*-

import requests
import json
import datetime
import random
import time

# server酱开关
sever = 'on'

# 填写server酱sckey
sckey = 'SCT165170TXTxIkK1P6SgzX6SxVEKwdGrL'

# 填入glados账号对应cookie
cookie = '_ga=GA1.2.1208787614.1656079546; koa:sess=eyJ1c2VySWQiOjE2OTI3OSwiX2V4cGlyZSI6MTY4MjA4NjQ5Nzg2NCwiX21heEFnZSI6MjU5MjAwMDAwMDB9; koa:sess.sig=tiw1p2X-ezGAHvHiUCVaindaR_w; __stripe_mid=a9fe374a-69dd-4f7b-aa62-0a09fc0255fe5195e3; _gid=GA1.2.695741567.1660364281; __cf_bm=tR40TeXGAr1QeAmaaEwzL12.qgVcbIevcTTHObllmb8-1660366331-0-Af/FhzPqMXK3+KIVyAT9sPfZHiUY6779AkXzByQxXdEgl5HtXdG/xcYcREH7Xp8WvUfSuGoXeyJhxnqOYx2093/mwybiVzNg3SqT8LLo76fedCKr6Ud9gn+jdSWBw9LGKw=='

def start():
    url = "https://glados.rocks/api/user/checkin"
    url2 = "https://glados.rocks/api/user/status"
    origin = "https://glados.rocks"
    referer = "https://glados.rocks/console/checkin"
    useragent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36"
    payload = {
        'token': 'glados.network'
    }

    checkin = requests.post(
        url,
        headers={
            'cookie': cookie, 'referer': referer,
            'origin': origin,
            'user-agent': useragent,
            'content-type': 'application/json;charset=UTF-8'
        },
        data=json.dumps(payload)
    )

    state = requests.get(url2, headers={
        'cookie': cookie,
        'referer': referer,
        'origin': origin,
        'user-agent': useragent
    })

    if 'message' in checkin.text:
        mess = checkin.json()['message']
        time = state.json()['data']['leftDays']
        time = time.split('.')[0]

    today = datetime.datetime.today()
    title = 'glados checkin (' + time + ' days)'
    desp = mess + ', you have ' + time + ' leftDays'
    print("title: %s desp: %s" % (title, desp))
    
    if sever == 'on':
        requests.get('https://sc.ftqq.com/' + sckey + '.send?title=' + title + '&desp=' + desp)
    else:
        requests.get('https://sc.ftqq.com/' + sckey + '.send?title=cookie expires!')
    
def main_handler(event, context):
    return start()

if __name__ == '__main__':
    print("****************************************************************************************")
    today = datetime.datetime.today()
    print("begin datetime: %s" % today.strftime('%Y/%m/%d %H:%M:%S'))

    # number = random.randint(0, 60 * 60)
    number = random.randint(0, 5)
    time.sleep(number)
    print("sleep: %d seconds" % number)

    start()

    today = datetime.datetime.today()
    print("end datetime: %s" % today.strftime('%Y/%m/%d %H:%M:%S'))
    print("****************************************************************************************")
