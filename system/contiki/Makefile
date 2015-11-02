CONTIKI = src
TARGET=jn516x
CPU=
DEV=
CHIP ?= jn5168
OUT_INCDIR=include
OUT_LIBDIR=lib





HEADER_DIRS=$(CONTIKI)/core $(CONTIKI)/platform/$(TARGET) $(foreach cpu,$(CPU),$(CONTIKI)/cpu/$(cpu)) $(foreach dev,$(DEV),$(CONTIKI)/dev/$(dev))
SRC_HEADERS=$(shell sh -c "$(foreach incdir,$(HEADER_DIRS),find $(incdir) -name \"*.h\" ;)")
OUT_HEADERS=$(foreach header,$(SRC_HEADERS), $(subst $(CONTIKI)/,$(OUT_INCDIR)/,$(header))) $(OUT_INCDIR)/$(PRJCONF_HEADER)

PRJCONF_HEADER=contiki-project-conf.h
CFLAGS += -DPROJECT_CONF_H=\"$(PRJCONF_HEADER)\"

all: $(OUT_LIBDIR)/contiki-$(CHIP).a $(OUT_HEADERS)

clean: clean_prj

.PHONY: clean_prj
clean_prj:
	rm -rf $(OUT_LIBDIR) $(OUT_INCDIR)

$(OUT_INCDIR)/$(PRJCONF_HEADER) : $(PRJCONF_HEADER)
	@if [ ! -d `dirname $@` ] ; then echo mkdir -p `dirname $@` ; mkdir -p `dirname $@` ; fi
	cp -p $< $@

$(OUT_INCDIR)/%.h : $(CONTIKI)/%.h
	@if [ ! -d `dirname $@` ] ; then echo mkdir -p `dirname $@` ; mkdir -p `dirname $@` ; fi
	cp -p $< $@

$(OUT_LIBDIR)/contiki-$(CHIP).a: contiki-$(TARGET).a
	@if [ ! -d `dirname $@` ] ; then echo mkdir -p `dirname $@` ; mkdir -p `dirname $@` ; fi
	cp -p $< $@

include $(CONTIKI)/Makefile.include
