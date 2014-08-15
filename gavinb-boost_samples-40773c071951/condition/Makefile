
# Boost

BOOST_LIBS=-lboost_thread-mt -lboost_system
LDFLAGS=$(BOOST_LIBS)

# condition examples

TARGETS=producer_consumer producer_consumer2
OBJS=$(TARGETS:%=%.o)

all: 			$(TARGETS)

clean:			#
			rm -f $(TARGETS) $(OBJS)
