#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------
#define	STRING_LENGTH		20
#define	SAMPLE_SIZE			400
#define	CROSSOVERS			100
#define	MUTATORS			40

//----------------------------------------------------------------------------------------------------
char	best[STRING_LENGTH];
int		bestfit = 0;

//----------------------------------------------------------------------------------------------------
char	gCube[24];

//----------------------------------------------------------------------------------------------------
inline	int		RandomVal (int size)
{
	return (rand () * size) / (RAND_MAX + 1);
}

//----------------------------------------------------------------------------------------------------
void	PrintCube (char cube[24])
{
	fprintf (stderr, "      +-----+\n");
	fprintf (stderr, "      |%02d %02d|\n", cube[0], cube[1]);
	fprintf (stderr, "      |%02d %02d|\n", cube[2], cube[3]);
	fprintf (stderr, "+-----+-----+-----+\n");
	fprintf (stderr, "|%02d %02d|%02d %02d|%02d %02d|\n", cube[4], cube[5], cube[8], cube[9], cube[12], cube[13]);
	fprintf (stderr, "|%02d %02d|%02d %02d|%02d %02d|\n", cube[6], cube[7], cube[10], cube[11], cube[14], cube[15]);
	fprintf (stderr, "+-----+-----+-----+\n");
	fprintf (stderr, "      |%02d %02d|\n", cube[16], cube[17]);
	fprintf (stderr, "      |%02d %02d|\n", cube[18], cube[19]);
	fprintf (stderr, "      +-----+\n");
	fprintf (stderr, "      |%02d %02d|\n", cube[20], cube[21]);
	fprintf (stderr, "      |%02d %02d|\n", cube[22], cube[23]);
	fprintf (stderr, "      +-----+\n");
}

//----------------------------------------------------------------------------------------------------
void	MoveCube (char cube[24], int move)
{
	int	temp, temp2;
	switch (move)
	{
		case 0:	//	x + (top counter-clockwise)
			temp = cube[4];
			temp2 = cube[5];
			cube[4] = cube[23];
			cube[5] = cube[22];
			cube[23] = cube[12];
			cube[22] = cube[13];
			cube[12] = cube[8];
			cube[13] = cube[9];
			cube[8] = temp;
			cube[9] = temp2;
			temp = cube[3];
			cube[3] = cube[2];
			cube[2] = cube[0];
			cube[0] = cube[1];
			cube[1] = temp;
			break;
		case 1:	//	x - (top clockwise)
			temp = cube[4];
			temp2 = cube[5];
			cube[4] = cube[8];
			cube[5] = cube[9];
			cube[8] = cube[12];
			cube[9] = cube[13];
			cube[12] = cube[23];
			cube[13] = cube[22];
			cube[23] = temp;
			cube[22] = temp2;
			temp = cube[3];
			cube[3] = cube[1];
			cube[1] = cube[0];
			cube[0] = cube[2];
			cube[2] = temp;
			break;
		case 2:	//	y + (right clockwise)
			temp = cube[1];
			temp2 = cube[3];
			cube[1] = cube[9];
			cube[3] = cube[11];
			cube[9] = cube[17];
			cube[11] = cube[19];
			cube[17] = cube[21];
			cube[19] = cube[23];
			cube[21] = temp;
			cube[23] = temp2;
			temp = cube[12];
			cube[12] = cube[14];
			cube[14] = cube[15];
			cube[15] = cube[13];
			cube[13] = temp;
			break;
		case 3:	//	y - (right counter-clockwise)
			temp = cube[1];
			temp2 = cube[3];
			cube[1] = cube[21];
			cube[3] = cube[23];
			cube[21] = cube[17];
			cube[23] = cube[19];
			cube[17] = cube[9];
			cube[19] = cube[11];
			cube[9] = temp;
			cube[11] = temp2;
			temp = cube[12];
			cube[12] = cube[13];
			cube[13] = cube[15];
			cube[15] = cube[14];
			cube[14] = temp;
			break;
		case 4:	//	z + (front clockwise)
			temp = cube[2];
			temp2 = cube[3];
			cube[2] = cube[7];
			cube[3] = cube[5];
			cube[7] = cube[17];
			cube[5] = cube[16];
			cube[17] = cube[12];
			cube[16] = cube[14];
			cube[12] = temp;
			cube[14] = temp2;
			temp = cube[8];
			cube[8] = cube[10];
			cube[10] = cube[11];
			cube[11] = cube[9];
			cube[9] = temp;
			break;
		case 5:	//	z - (front counter-clockwise)
			temp = cube[2];
			temp2 = cube[3];
			cube[2] = cube[12];
			cube[3] = cube[14];
			cube[12] = cube[17];
			cube[14] = cube[16];
			cube[17] = cube[7];
			cube[16] = cube[5];
			cube[7] = temp;
			cube[5] = temp2;
			temp = cube[8];
			cube[8] = cube[9];
			cube[9] = cube[11];
			cube[11] = cube[10];
			cube[10] = temp;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------------------
int		KillEmptyPairs (char test[STRING_LENGTH])
{
	char	copy[STRING_LENGTH];
	int		copy_size = 0;
	int		i;
	for (i = 0; i < STRING_LENGTH; i++)
	{
		if (i < (STRING_LENGTH - 1))
			if (test[i] & 1)
			{
				if (test[i + 1] == test[i] - 1)
				{
					i++;
					continue;
				}
			}
			else
			{
				if (test[i + 1] == test[i] + 1)
				{
					i++;
					continue;
				}
			}
		copy[copy_size++] = test[i];
	}
	for (i = 0; i < copy_size; i++)
		test[i] = copy[i];
	for (; i < STRING_LENGTH; i++)
		test[i] = RandomVal (6);
	return copy_size;
}

//----------------------------------------------------------------------------------------------------
int		SubstituteLongStrings (char test[STRING_LENGTH])
{
	char	copy[STRING_LENGTH];
	int		copy_size = 0;
	int		i;
	for (i = 0; i < STRING_LENGTH; i++)
	{
		if (i < (STRING_LENGTH - 2))
		{
			int		m = i + 2;
			if ((test[i + 1] == test[i]) && (test[m] == test[i]))
			{
				if (test[m] & 1)
					test[m]--;
				else
					test[m]++;
				i++;
				continue;
			}
		}
		copy[copy_size++] = test[i];
	}
	for (i = 0; i < copy_size; i++)
		test[i] = copy[i];
	for (; i < STRING_LENGTH; i++)
		test[i] = RandomVal (6);
	return copy_size;
}

//----------------------------------------------------------------------------------------------------
void	Compress (char test[STRING_LENGTH])
{
	int		kill,
				sub;
	do
	{
		kill = KillEmptyPairs (test);
		sub = SubstituteLongStrings (test);
	}
	while ((kill < STRING_LENGTH) || (sub < STRING_LENGTH));
}

//----------------------------------------------------------------------------------------------------
void	PermuteCube (int count)
{
	std::cout << "Complexity " << count << '\n';
	int	i;
	for (i = 0; i < 24; i++)
		gCube[i] = i;
	PrintCube (gCube);
	char	array[STRING_LENGTH];
	std::cout << "Permute: ";
	for (i = 0; i < STRING_LENGTH; i++)
		array[i] = RandomVal (6);
	Compress (array);
	for (i = 0; i < count; i++)
	{
		std::cout << int (array[i]) << " ";
		MoveCube (gCube, array[i]);
		if (array[i] & 1)
			array[i]--;
		else
			array[i]++;
	}
	std::cout << "\nInverse: ";
	for (i = count - 1; i >= 0; i--)
		std::cout << int (array[i]) << " ";
	std::cout << "\n\n";
	std::cout.flush ();
}

//----------------------------------------------------------------------------------------------------
int		Fitness (char test[STRING_LENGTH])
{
	char	work[24];
	int	fitness = 0;
	int	i;
	for (i = 0; i < 24; i++)
		work[i] = gCube[i];
	for (i = 0; i < STRING_LENGTH; i++)
	{
		MoveCube (work, test[i]);
		int		fit = 0;
		for (int j = 0; j < 24; j++)
			if (work[j] == j)
				fit++;
		if (fit > fitness)
			fitness = fit;
	}
	return fitness;
}

//----------------------------------------------------------------------------------------------------
void	PrintIndividual (char test[STRING_LENGTH], int fitness)
{
	char	work[24];
	int		i;
	for (i = 0; i < 24; i++)
		work[i] = gCube[i];
	for (i = 0; i < STRING_LENGTH; i++)
	{
		std::cout << int (test[i]) << " ";
		MoveCube (work, test[i]);
		int		fit = 0;
		for (int j = 0; j < 24; j++)
			if (work[j] == j)
				fit++;
		if (fit == fitness)
			break;
	}
	std::cout << "\n    Fitness " << (fitness / 24.0) << "    Length " << (i + 1) << "\n\n";
	std::cout.flush ();
}

//----------------------------------------------------------------------------------------------------
void	PrintInstructions (char test[STRING_LENGTH], int fitness)
{
	char	work[24];
	int		i;
	for (i = 0; i < 24; i++)
		work[i] = gCube[i];
	char*   strings[6] = {
	                        "top counter-clockwise",
	                        "top clockwise",
	                        "right clockwise",
	                        "right counter-clockwise",
	                        "front clockwise",
	                        "front counter-clockwise"
	                    };
	std::cout << "Instructions" << "\n";
	std::cout << "------------" << "\n";
	for (i = 0; i < STRING_LENGTH; i++)
	{
		std::cout << strings[test[i]] << "\n";
		MoveCube (work, test[i]);
		int		fit = 0;
		for (int j = 0; j < 24; j++)
			if (work[j] == j)
				fit++;
		if (fit == fitness)
			break;
	}
	std::cout << "\n    Fitness " << (fitness / 24.0) << "    Length " << (i + 1) << "\n\n";
	std::cout.flush ();
}

//----------------------------------------------------------------------------------------------------
void	Crossover (char next[SAMPLE_SIZE][STRING_LENGTH], int count)
{
	for (int i = 0; i < count; i++)
	{
		int		a = RandomVal (SAMPLE_SIZE),
					b = RandomVal (SAMPLE_SIZE);
		for (int j = 0; j < STRING_LENGTH; j++)
		{
			char	tmpa = next[a][j],
						tmpb = next[b][j];
			next[a][j] = (rand () & 1) ? tmpa : tmpb;
			next[b][j] = (rand () & 1) ? tmpa : tmpb;
		}
	}
}

//----------------------------------------------------------------------------------------------------
void	Mutate (char next[SAMPLE_SIZE][STRING_LENGTH], int count)
{
	#if 0
	for (int i = 0; i < count; i++)
	{
		int		crossover = RandomVal (STRING_LENGTH),
					a = RandomVal (SAMPLE_SIZE);
		next[a][crossover] = RandomVal (6);
	}
	#else
	for (int i = 0; i < count; i++)
	{
		int		mutate1 = RandomVal (STRING_LENGTH),
					mutate2 = RandomVal (STRING_LENGTH),
					a = RandomVal (SAMPLE_SIZE),
					tmp = next[a][mutate1];
		next[a][mutate1] = next[a][mutate2];
		next[a][mutate2] = tmp;
	}
	#endif
}

//----------------------------------------------------------------------------------------------------
void	Generation (int gen, char last[SAMPLE_SIZE][STRING_LENGTH], char next[SAMPLE_SIZE][STRING_LENGTH])
{
//	loop over last, computing fitness
	int		fitness[SAMPLE_SIZE],
				total = 0,
				i;
	for (i = 0; i < SAMPLE_SIZE; i++)
	{
		Compress (last[i]);
		fitness[i] = Fitness (last[i]);
		if (fitness[i] > bestfit)
		{
			bestfit = fitness[i];
			for (int j = 0; j < STRING_LENGTH; j++)
				best[j] = last[i][j];
		}
		total += fitness[i];
	}
//	build a table of probabilities
	double	prob[SAMPLE_SIZE];
	for (i = 0; i < SAMPLE_SIZE; i++)
		prob[i] = fitness[i] / double (total);
//	transfer last to next using probabilites
	for (i = 0; i < SAMPLE_SIZE; i++)
	{
		double	x = rand () / double (RAND_MAX + 1);
		int			index = 0;
		while ((x > prob[index]) && (index < SAMPLE_SIZE))
		{
			x -= prob[index];
			index++;
		}
		if (index < SAMPLE_SIZE)
			for (int j = 0; j < STRING_LENGTH; j++)
				next[i][j] = last[index][j];
		else
			i--;
	}
	Crossover (next, CROSSOVERS >> 1);
	Mutate (next, MUTATORS);
}

//----------------------------------------------------------------------------------------------------
void	main (void)
{
	srand ((unsigned) time (NULL));
	char	test[2][SAMPLE_SIZE][STRING_LENGTH];
	//PermuteCube (STRING_LENGTH);
	
	gCube[ 0] =  0;
	gCube[ 1] =  1;
	gCube[ 2] =  2;
	gCube[ 3] =  3;
	gCube[ 4] =  4;
	gCube[ 5] =  5;
	gCube[ 6] =  6;
	gCube[ 7] =  7;
	gCube[ 8] =  8;
	gCube[ 9] =  9;
	gCube[10] = 10;
	gCube[11] = 19;
	gCube[12] = 12;
	gCube[13] = 13;
	gCube[14] = 15;
	gCube[15] = 14;
	gCube[16] = 16;
	gCube[17] = 21;
	gCube[18] = 18;
	gCube[19] = 11;
	gCube[20] = 20;
	gCube[21] = 17;
	gCube[22] = 22;
	gCube[23] = 23;
	
	int		count = 0;
	do
	{
		bestfit = 0;
		int	i;
		for (i = 0; i < SAMPLE_SIZE; i++)
			for (int j = 0; j < STRING_LENGTH; j++)
				test[0][i][j] = RandomVal (6);
		for (i = 0; (bestfit != 24) && (i < 128); i++)
		{
			int		last = i & 0x01,
						next = last ^ 0x01;
			Generation (i, test[last], test[next]);
			if ((i & 7) == 0)
				std::cerr << '.';
		}
		std::cout << '\n' << ++count << ": ";
		//PrintIndividual (best, bestfit);
	}
	while (bestfit != 24);
	PrintInstructions (best, bestfit);
}

//----------------------------------------------------------------------------------------------------
