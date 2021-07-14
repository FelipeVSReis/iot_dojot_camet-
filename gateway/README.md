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
more info in : https://oldwiki.archive.openwrt.org/doc/techref/opkg

Com esses comandos temos que a versão foi atualizada para: 
`Atualização de firmware para a versão: v4.3.7-20190911-2232`

Teste de Comunicação simples com a Dojot


Diferenças entre as versões dojot para se atentar no momento que configura o mqtt para recebimento de pacotes.
+ /admin/device/attrs
+ admin:device/attrs

Algumas outras referências utilizadas e possíveis de serem utilizadas:
1. https://wiki.dragino.com/index.php?title=Main_Page
2. https://www.wiki.dragino.com/index.php?title=Through_MQTT_to_upload_data
3. https://wiki.dragino.com/index.php?title=TCP_Connection_Instruction
4. https://wiki.dragino.com/index.php?title=Examples_Catalog
5. https://wiki.dragino.com/index.php?title=Mesh_IoT_Firmware

Hex to Char
1. https://www.rapidtables.com/convert/number/hex-to-ascii.html#:~:text=Hex%20to%20ASCII%20text%20conversion%20table%20%20,%20%20ETX%20%20124%20more%20rows%20
2. https://calculareconverter.com.br/hex-to-ascii/

Biblioteca RadioHead
1. https://github.com/dragino/RadioHead/blob/master/RH_RF95.h
2. https://github.com/dragino/RadioHead/blob/master/RH_RF95.cpp

https://github.com/dragino/Arduino-Profile-Examples/blob/master/libraries/Dragino/examples/LoRa/LoRaWAN/single_pkt_fwd_v003/single_pkt_fwd_v003.ino
https://github.com/dragino/Arduino-Profile-Examples/blob/master/libraries/Dragino/examples/LoRa/MQTT_Simple_Server/MQTT_Simple_Server.ino
