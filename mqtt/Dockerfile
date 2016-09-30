FROM rabbitmq:3.6

RUN rabbitmq-plugins enable --offline rabbitmq_management \
    && rabbitmq-plugins enable --offline rabbitmq_mqtt

EXPOSE 15672 1883
