# Web Caching in C
## Threaded Main Server
## Threaded Proxy Server

1)`cd main_server/`

2)`gcc main_server.c -o main_server -lpthread`

3)`./main_server 1200`

4)`press <control> + z`

5)`cd ../proxy_server/`

6)`gcc proxy_server.c -o proxy_server -lpthread`

7)`./proxy_server 0.0.0.0 1200 3000`

8)`press <control> + z`

Type [http://localhost:3000/](http://localhost:3000/) in the browser

##### note: 
Only the following links will work
[http://localhost:3000/](http://localhost:3000/)
[http://localhost:3000/index.html](http://localhost:3000/index.html)
[http://localhost:3000/jumla.html](http://localhost:3000/jumla.html)

Rest will give <b>404 Error</b>

