<?php
require( "./phpMQTT.php" );

function makeSentense($message, $shouldEndSession){
    return array(
        'version' => "1.0",
        'sessionAttributes' => null,
        'response' => [
            'outputSpeech' =>[
                'type' => "SimpleSpeech",
                'values' => [
                    'lang' => 'ja',
                    'type' => 'PlainText',
                    'value' => $message
                ]
            ],
            'directives' => null,
            'shouldEndSession' => $shouldEndSession
        ]
    );
}

$json_string = file_get_contents('php://input');
$jsonObj = json_decode($json_string);

// log output
error_log(var_export($jsonObj, true));

$message = $jsonObj->{"request"}->{"intent"}->{"slots"}->{"any"} ->{"value"}; 
$mqtt_host = "www.hirotakaster.com";
$mqtt_port = 1883;
$mqtt_clientid = "clientid";
$mqtt_topic = "";

if ($message == "輪郭") {
    $mqtt_topic = "opencv/";
    $message = "オープンシーブイの画像を取得します";
} else if ($message == "画像") {
    $mqtt_topic = "image/";
    $message = "RGBの画像を取得します";
}
$mqtt_message = '1';
$mqtt = new Bluerhinos\phpMQTT($mqtt_host, $mqtt_port, $mqtt_clientid );
if( $mqtt->connect() ){
  $mqtt->publish( $mqtt_topic, $mqtt_message, 0 );
  $mqtt->close();
}

$response = makeSentense($message, false);

header("Content-Type: application/json; charset=utf-8");
echo json_encode($response);
?>
