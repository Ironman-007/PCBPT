.PHONY: all host

all: host

host:
	python3 -m gunicorn --worker-class eventlet -w 1 interface:app --reload --bind :8001
