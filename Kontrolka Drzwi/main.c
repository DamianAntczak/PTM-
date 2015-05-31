void IOToggle(void);

int main(void)
{
		SystemInit();
		SystemCoreClockUpdate(); // inicjalizacja dystrybucji czasu procesora

    //automatically added by CoIDE
	IOToggle();

	while(1)
    {
    }
}

