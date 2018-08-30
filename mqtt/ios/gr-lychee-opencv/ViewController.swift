import UIKit
import Moscapsule

class ViewController: UIViewController {
    
    let mqttConfig = MQTTConfig(clientId: "testid", host: "your mqtt server", port: 1883, keepAlive: 60)
    var mqttClient: MQTTClient!

    @IBOutlet weak var outputImage: UIImageView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        mqttConfig.onConnectCallback = { returnCode in
            NSLog("Return Code is \(returnCode.description)")
        }
        
        mqttConfig.onMessageCallback = { message in
            NSLog("mqtt call back \(message.topic)")
            let data = (message.payload)!
            let image: UIImage? = UIImage(data: data)
            
            DispatchQueue.main.async {
                self.outputImage.image = image
            }
        }
        
        mqttClient =  MQTT.newConnection(mqttConfig)
        mqttClient.subscribe("opencv/output", qos: 2)

    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func onclickcanny(_ sender: Any) {
        mqttClient.publish(string: "1", topic: "opencv/", qos: 1, retain: false)
    }

    
    @IBAction func onclickdrawing(_ sender: Any) {
        mqttClient.publish(string: "2", topic: "opencv/", qos: 2, retain: false)
    }
}

