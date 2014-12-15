
.PHONY: test
test: nth
	$(MAKE) -C nth-gtest

.PHONY: nth
nth:
	$(MAKE) -C nth

.PHONY: clean
clean:
	$(MAKE) -C nth-gtest clean
	$(MAKE) -C nth clean
