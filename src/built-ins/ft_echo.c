int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	i = 0;
	while (ft_strcmp("echo", argv[i]))
		i++;
	if (!ft_strncmp("-n", argv[++i], 3))
		newline = 0;
	else
		newline = 1;
	
	return (0);
}
