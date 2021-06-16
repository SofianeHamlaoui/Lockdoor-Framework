FROM kalilinux/kali
MAINTAINER Sofiane Hamlaoui <s0fianehaml40u1@gmail.com> <https://github.com/SofianeHamlaoui>

LABEL org.label-schema.name="sofianehamlaoui/lockdoor"
LABEL org.label-schema.description="Lockdoor Framework : A Penetration Testing framework with Cyber Security Resources"
LABEL org.label-schema.usage="https://github.com/SofianeHamlaoui/Lockdoor-Framework"
LABEL org.label-schema.url="https://lockdoor.sofianehamlaoui.fr/"
LABEL org.label-schema.docker.cmd="docker run -it --name lockdoor-container -w /home/Lockdoor-Framework --net=host sofianehamlaoui/lockdoor"



WORKDIR /root
RUN apt-get update && \
    apt-get install -y git wget curl && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* && \
    echo 'export LANG="en_US.UTF-8"' >> .bashrc && \
    curl -L https://gist.githubusercontent.com/SofianeHamlaoui/a95285afd01523f7d7531b002fa669db/raw/0dc9cb9084158aafd0dc3eb3481824242d408fb9/bashrc >> .bashrc && \
    git clone https://github.com/SofianeHamlaoui/Lockdoor-Framework.git && \
    apt update && apt upgrade


ENV PYTHONIOENCODING utf-8

WORKDIR /root/Lockdoor-Framework

RUN ./install.sh
