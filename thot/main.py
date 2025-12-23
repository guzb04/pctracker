import requests
import json
import time
from bs4 import BeautifulSoup

i = 1
while True:
    r = requests.get(
        f"https://thotcomputacion.com.uy/wp-json/wp/v2/product?page={i}")
    data = r.json()
    time.sleep(1)

    if len(data) > 0:
        if (len(data) == 3) and data['data']['status'] == 400:
            break

        datalist = []
        for j in data:
            httpResponse = requests.get(j['link'], stream=True)
            category = requests.get(
                f"https://thotcomputacion.com.uy/wp-json/wp/v2/product_cat/{j['product_cat'][0]}")
            categoryParsed = category.json()['name']
            rawData = httpResponse.text
            file = open("temp.html", "w")
            file.write(rawData)
            with open("temp.html") as tempHtml:
                soup = BeautifulSoup(tempHtml, features='html.parser')
                price = soup.find_all('bdi')
                priceString = str(price[0])
                newPrice = priceString[62:-6]

            dictToExport = {
                "title":  j['title']['rendered'],
                "price": newPrice,
                "category": categoryParsed
            }

            jsonReturn = json.dumps(dictToExport)
            print(jsonReturn, flush=True)
            time.sleep(1)

        i = i+1
