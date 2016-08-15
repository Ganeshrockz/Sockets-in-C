# Web Caching in C
## Threaded Main Server
## Threaded Proxy Server

1)`git clone https://github.com/prabhakaran9397/Sockets-in-C.git`

2)`cd Sockets-in-C/web_caching/main_server/`

3)`gcc main_server.c -o main_server -lpthread`

4)`./main_server 1200`

5)`press <control> + z`

6)`cd ../proxy_server/`

7)`gcc proxy_server.c -o proxy_server -lpthread`

8)`./proxy_server 0.0.0.0 1200 3000`

9)`press <control> + z`

Type [http://localhost:3000/](http://localhost:3000/) in the browser

##### note: 
Only the following links will work

[http://localhost:3000/](http://localhost:3000/)

[http://localhost:3000/index.html](http://localhost:3000/index.html)

[http://localhost:3000/jumla.html](http://localhost:3000/jumla.html)

Rest will give <b>404 Error</b>

