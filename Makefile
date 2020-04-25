all:
	+$(MAKE) -C quarto
	+$(MAKE) -C utentes

clean:
	+$(MAKE) clean -C quarto
	+$(MAKE) clean -C utentes