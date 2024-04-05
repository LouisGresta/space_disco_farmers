ADRESS="192.168.43.152"
PORT="12346"
SERIAL="/dev/cu.usbserial-AD0K8WZ8"
TEAM_NAME="SDF"

python3 -m space_collector.serial2tcp -a $ADRESS -p $PORT --serial $SERIAL --team-name $TEAM_NAME
