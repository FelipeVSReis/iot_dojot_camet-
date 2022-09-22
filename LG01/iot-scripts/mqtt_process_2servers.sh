#!/bin/sh

#Check If we have set debug
DEBUG=`uci get iot-services.general.debug`
DEBUG=`echo $DEBUG| awk '{print int($0)}'`

UPDATE_INTERVAL=5

old=`date +%s`

server_type=`uci get mqtt.general.server_type`

# Check if the MQTT Server is pre_config
if [ `uci get mqtt.$server_type` == "general" ];then
        pre_config_server=0
else
        pre_config_server=1
fi

#Set MQTT Server Address [-h]
if [ $pre_config_server -eq 0 ];then
        server=`uci get mqtt.general.server`
else
        server=`uci get mqtt.$server_type.server`
fi                                           
                                          
#Set MQTT Server Port [-p]               
if [ $pre_config_server -eq 0 ];then     
        port=`uci get mqtt.general.port`
else                                          
        port=`uci get mqtt.$server_type.port`
fi                                                    
                                                      
#Set MQTT Parameter [-u][-P][-i]                           
user=`uci get mqtt.general.user_name`                      
pass=`uci get mqtt.general.password`                  
clientID=`uci get mqtt.general.client_id`
                                                           
                                                      
#Set MQTT Publish topic format                        
if [ $pre_config_server -eq 0 ];then                       
        pub_format=`uci get mqtt.general.topic_format`     
else                                                  
        pub_format=`uci get mqtt.$server_type.topic_format` 
fi                                                    
                                              
#Set MQTT Publish data format                              
if [ $pre_config_server -eq 0 ];then                  
        data_format=`uci get mqtt.general.data_format`
else                                                       
        data_format=`uci get mqtt.$server_type.data_format`
fi                                                    
                                                           
[ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "Server:Port" $server:$port
[ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "Topic Format: " $pub_format
[ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "Data Format: " $data_format
                                                                     
                                                                      
#Run Forever                                            
              
while [ 1 ]                                                          
     do                                                                    
        now=`date +%s`                                                
        if [ `expr $now - $old` -gt $UPDATE_INTERVAL ];then          
                old=`date +%s`                                        
                CID=`ls /var/iot/channels/`                           
                [ $DEBUG -ge 2 ] && logger "[IoT.MQTT]: " "Check for sensor update"
                [ $DEBUG -ge 2 ] && logger "[IoT.MQTT]: " "Found Local Channels:" $CID
                for channel in $CID; do                               
                        if [ "`uci get mqtt.$channel.local_id`" == "$channel" ]; then
                                [ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "Find Match Entry for $channel"         
        			[ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "[-t] $pub_format"	
                                                                            
                                #General MQTT Update Data                       
                                data=`cat /var/iot/channels/$channel`           
                                mqtt_data=`echo ${data_format/DATA/$data}`  
                                [ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "[-m] $mqtt_data"
                                                                                
                                ### Send MQTT Command                           
                                mosquitto_pub -h $server -p $port -u $user -t $pub_format -m $DATA

                                                                                
                                ### Delete the Channel info  
                                rm /var/iot/channels/$channel                   
                        else      
                                 [ $DEBUG -ge 1 ] && logger "[IoT.MQTT]: " "Do Not Find Match Entry for $channel"                                          
                                rm /var/iot/channels/$channel                   
                        fi                                                      
                #done                                                            
        fi                                                                      
done