# c-http-server

Simple (WIP) implementation of an HTTP server in the C programming language. Made for educational purposes to learn C and some network programming concepts.

## Building the Project

To build the project, navigate to the project root directory and run the following command:

```bash
make
```

This will generate the executable in the build directory.

## Running the HTTP Server

After building the project, you can run the HTTP server using the following command:

```bash
./build/http_server <port_num>
```

By default, the server listens on port 8080. You can optionally pass a port number as an argument of `http_server` to bind it to a different port.

## TODO List

- [x] Implement parsing of HTTP headers.
- [ ] Handle HTTP request bodies.
- [ ] Process HTTP request resources.
- [ ] Add error handling and response codes.
- [ ] Implement basic routing and request handling.
- [ ] Support for static file serving.
- [ ] Implement multi-threading or asynchronous processing for handling multiple connections.
- [ ] Add logging and debugging features.
- [ ] Implement HTTPS support.