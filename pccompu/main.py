import time
import requests
from bs4 import BeautifulSoup
from bs4 import XMLParsedAsHTMLWarning
import warnings

warnings.filterwarnings("ignore", category=XMLParsedAsHTMLWarning)


def getUrls(urls):
    urlList = []
    for url in urls:
        url = url.text.strip()
        if url.startswith(PREFIX):
            parsedText = url[len(PREFIX):]
            if (parsedText.startswith("memoria-ram") or
                    parsedText.startswith("tarjetas-de-video)") or
                    parsedText.startswith("almacenamiento") or
                    parsedText.startswith("procesadores")):
                urlList.append(parsedText)
    return urlList


PREFIX = "http://www.pccompu.com.uy/catalogo/componentes/"

r = requests.get("https://www.pccompu.com.uy/sitemap.xml")
data = r.text

file = open("sitemap.xml", "w")
file.write(data)


with open("sitemap.xml") as siteMap:
    soup = BeautifulSoup(siteMap, features='lxml')


for priority in soup.find_all("priority"):
    priority.decompose()

urls = soup.find_all("url")

componentList = getUrls(urls)

for component in componentList:
    componentRequest = requests.get(f"{PREFIX}{component}")
    time.sleep(1)
    componentFile = open("tempComponent.html", "w")
    componentFile.write(componentRequest.text)

    with open("tempComponent.html") as componentHTML:
        csoup = BeautifulSoup(componentHTML, features='html.parser')
        name = csoup.find_all("h1", class_="nombre")[0].text
        precio = csoup.find_all("span", id="precio_ent_actual")[0].text
        link = f"{PREFIX}{component}"
        categoria = component.split("/")[0]
        print(name, precio, categoria, link, flush=True)
