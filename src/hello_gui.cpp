#include "hello_gui.h"
#include "ui_hello_gui.h"

HelloGui::HelloGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::HelloGui)
{
  ui->setupUi(this);

  nh_.reset(new ros::NodeHandle("~"));

  // setup the timer that will signal ros stuff to happen
  ros_timer = new QTimer(this);
  connect(ros_timer, SIGNAL(timeout()), this, SLOT(spinOnce()));
  ros_timer->start(100);  // set the rate to 100ms  You can change this if you want to increase/decrease update rate


  // setup subscriber by according to the ~/chatter_topic param
  std::string listen_topic;
  nh_->param<std::string>("listen_topic",listen_topic,"/talker/chatter");
  chatter_sub_ = nh_->subscribe<std_msgs::String>(listen_topic, 1, &HelloGui::chatterCallback, this);

  // publish a message on the channel specified by ~/hello_topic param
  std::string hello_topic;
  nh_->param<std::string>("hello_topic",hello_topic,"chatter");
  hello_pub_ = nh_->advertise<std_msgs::String>(hello_topic,1);
}

HelloGui::~HelloGui()
{
  delete ui;
  delete ros_timer;
}


void HelloGui::spinOnce(){
  if(ros::ok()){
    ros::spinOnce();
  }
  else
      QApplication::quit();
}


void HelloGui::chatterCallback(const std_msgs::String::ConstPtr &msg){
  auto qstring_msg = QString::fromStdString( msg->data.c_str() );

  ui->chatter->setText(qstring_msg);
}

void HelloGui::on_hi_button_clicked()
{
  std_msgs::String msg;
  std::stringstream ss;
  ss << "hello world " << ui->hi_num->value();
  msg.data = ss.str();

  hello_pub_.publish(msg);

  ui->hi_num->setValue(ui->hi_num->value()+1);
}
