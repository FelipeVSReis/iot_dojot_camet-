# Dragino LG-01
Para utilização desse gateway foi necessário alguns passos antes, alguns presentes na documentação de forma direta, outros indo em busca de algumas variações de outros códigos presentes no site da própria dragino;

Para desenvolvimento desses códigos foi utilizado o dragino-starter-kit v1;

Para comunicação via ssh como o LG-01:
+ Pós configuração inicial e conexão do gateway com a internet é necessário saber o IP dele para conexão via ssh. Comando que utilizo para verficar os IP conectados na rede `nmap -sn`
+ Já para a conexão via SSH, esse modelo é um tanto quanto antigo então foi preciso um comando um pouco mais específico para conexão via ssh
```bash
ssh -oKexAlgorithms=+diffie-hellman-group1-sha1 root@insira.ip.aqui.00 
```

Inicialmente foi necessário atualizar o firmware do LG-01 para a versão mais atual disponível pela dragino
```language
opkg update
opkg list
opkg install mosquitto-client
```
Com esses comandos temos que a versão foi atualizada para: 
`Atualização de firmware para a versão: v4.3.7-20190911-2232`

Teste de Comunicação simples com a Dojot


Diferenças entre as versões dojot para se atentar no momento que configura o mqtt para recebimento de pacotes.
+ /admin/device/attrs
+ admin:device/attrs