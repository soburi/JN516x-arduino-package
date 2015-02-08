extern int main(void);

extern "C"  void AppColdStart(void)
{
	main();
}

extern "C" void AppWarmStart(void)
{
	AppColdStart();
}


