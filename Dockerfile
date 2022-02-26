FROM ghcr.io/maxotta/dev-builder-c:latest AS builder
ARG CACHEBUST=0
COPY ./ /tmp/kiv-psi-task03-silhavyj/
RUN cd /tmp/kiv-psi-task03-silhavyj ; \
    cmake -S . -B build ; \
    cmake --build build ; \
    mkdir /app ; \
    cp build/kiv-psi-task03-silhavyj /app ;

FROM ghcr.io/maxotta/kiv-psi-base-docker:latest
COPY --from=builder /app/ /app/
ENTRYPOINT /app/kiv-psi-task03-silhavyj