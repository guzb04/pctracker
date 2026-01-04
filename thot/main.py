import requests
import json
import time
from bs4 import BeautifulSoup

i = 1

"""
23 - Procesadores AMD
24 - Procesadores Intel
27 - DDR3
28 - DDR4
29 - SODIMM
38 - Nvidia
39 - Radeon
495 - DDR5
"""

includedCategories = [27, 28, 29, 38, 39, 23, 24, 495]

for i in includedCategories:
    r = requests.get(
        f"https://thotcomputacion.com.uy/wp-json/wp/v2/product?product_cat={i}&per_page=100")
    data = r.json()

    time.sleep(1)

    if len(data) > 0:
        if (len(data) == 3) and data['data']['status'] == 400:
            break

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
                "category": categoryParsed,
                "link": j["link"]
            }

            jsonReturn = json.dumps(dictToExport)
            print(jsonReturn, flush=True)
            time.sleep(1.5)

        i = i+1
