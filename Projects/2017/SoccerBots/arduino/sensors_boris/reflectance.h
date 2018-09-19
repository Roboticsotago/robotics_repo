const int reflectancePin =  A8;
int reflectance;

void readReflectance() {
  reflectance = analogRead(reflectancePin);
}
