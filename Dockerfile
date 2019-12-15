FROM sofianehamlaoui/lockdoor

MAINTAINER Sofiane Hamlaoui <s0fianehaml40u1@gmail.com> <https://Github.com/SofianeHamlaoui>

LABEL org.label-schema.name="sofianehamlaoui/lockdoor"
LABEL org.label-schema.description="Lockdoor Framework : A Penetration Testing framework with Cyber Security Resources"
LABEL org.label-schema.usage="https://github.com/SofianeHamlaoui/Lockdoor-Framework"
LABEL org.label-schema.url="https://lockdoor.sofianehamlaoui.me"
LABEL org.label-schema.docker.cmd="docker run -it --name lockdoor-container -w /home/Lockdoor-Framework --net=host sofianehamlaoui/lockdoor"



WORKDIR /home/Lockdoor-Framework

RUN lockdoor