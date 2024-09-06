FROM alpine:3.20.2
RUN apk add build-base graphviz doxygen python3 py3-pip
RUN pip install cpplint==1.6.1 --break-system-packages
WORKDIR /test
ADD lib lib
ADD tests tests
COPY makefile .
COPY config.dox .
COPY README.md .
RUN make all