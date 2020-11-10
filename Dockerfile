FROM gcc:10
WORKDIR /app/
COPY ./* ./
RUN gcc suanfu.c -o suanfu
RUN chmod +x suanfu