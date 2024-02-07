#ifdef DEBUG_CHECK

#include <math.h>
#include "../include/stack.h"

static const char *gnt_polynomial = "00000100110000010001110110110111";
static const size_t polynomial_degree = 32;
static const size_t len_message = 512;

HASH_TYPE hash_control(Stack *stk)
{
	assert(stk != NULL);

	char *msg = (char *)calloc(len_message, sizeof(char));
	assert(msg != NULL);

	msg = convert_bin_msg(stk, msg);

	HASH_TYPE hash = hash_crc_32(msg);

	free(msg);
	msg = NULL;

return hash;
}

char *convert_bin_msg(Stack *stk, char *msg)
{
	assert(stk != NULL);
	assert(msg != NULL);

	size_t len = 0;
	len = conversion_bin_sys(stk->capacity, msg, len);
	len = conversion_bin_sys(stk->size, msg, len);

	for (size_t i = 0; i < stk->size; i++)
	len = conversion_bin_sys((size_t)stk->data[i], msg, len);

	return msg;
}

size_t conversion_bin_sys(size_t value, char *text, size_t text_len)
{
	assert(text != NULL);

	size_t bin = 0;
	size_t x = 1;
	size_t len = 0;

	while (value > 0)
	{
		bin += x * (value % 2);
		x *= 10;
		value /= 2;
		len++;
	}

	for (size_t i = 0; i < len; i++)
	{
		text[i + text_len] = (char)(bin % 10 + 48);
		bin /= 10;
	}

	char temp = '0';

	for (size_t i = 0; i < len / 2; i++)
	{
		temp = text[i + text_len];
		text[i + text_len] = text[len - 1 - i + text_len];
		text[len - 1 - i + text_len] = temp;
	}

	text_len += len;

	return text_len;
}

HASH_TYPE hash_crc_32(char *message)
{
	assert(message != NULL);

	char *reg   = (char *)calloc(polynomial_degree + 1, sizeof(char));
	assert(reg != NULL);

	reg[polynomial_degree] = '\0';
	memset(reg, '0', polynomial_degree * sizeof(char));

	size_t len_mes = strlen(message);

	for (size_t i = 0; i < len_mes; i++)
	{
		int extend_bit = reg[0];
		for (size_t j = 0; j < polynomial_degree - 1; j++)
			reg[j] = reg[j + 1];

		reg[polynomial_degree - 1] = message[i];

		if (extend_bit == '1')
		{
			for (size_t j = 0; j < polynomial_degree; j++)
			{
				if (gnt_polynomial[j] == '1')
				{
					if (reg[j] == '1')
						reg[j] = '0';
					else
						reg[j] = '1';
				}
			}
		}
	}

	HASH_TYPE hash = 0;
	int degree     = polynomial_degree;

	for (size_t i = 0; i < polynomial_degree; i++)
	{
		hash += (HASH_TYPE)(reg[i] * pow(3, degree));
		degree--;
	}

	free(reg);
	
	return hash;
}

#endif
