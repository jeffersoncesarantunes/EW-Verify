FROM alpine:latest AS build

RUN apk add --no-cache \
    gcc \
    musl-dev \
    make \
    linux-headers

WORKDIR /src
COPY . .
RUN make clean && make

FROM alpine:latest

RUN apk add --no-cache \
    iw \
    rfkill \
    wireless-tools \
    ca-certificates

COPY --from=build /src/ewverify /usr/local/bin/ewverify

ENTRYPOINT ["ewverify"]
CMD ["--help"]
