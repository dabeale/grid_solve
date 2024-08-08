FROM alpine:3.20.2
RUN apk add build-base graphviz doxygen
WORKDIR /test
ADD lib lib
ADD tests tests
COPY makefile .
COPY config.dox .
RUN make all
ENTRYPOINT [ "/test/bin/test_release" ]