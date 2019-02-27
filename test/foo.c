unsigned char
test_char(unsigned char b)
{
	return b;
}

unsigned short
test_short(unsigned short b)
{
	return b;
}

int
test(int k)
{
	return k;
}

int
test_multiple_int(int j, int v)
{
	return 0;
}

int 
test_multiple_types(int j, short u)
{
	return j;
}

// This should xform 2 to a local var
// Then inject zap it
// Then replace the local var with zapped var
int
main(int argc, char **argv)
{
	test_char(3);
	test_short(4);
	test_multiple_int(2,3);
	test_multiple_types(2,3);
	return test(2);
}
