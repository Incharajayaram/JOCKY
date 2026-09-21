# JOCKY — Clean Architecture

**Phase 1: Compiler & Obfuscation Pipeline**

## Overview
JOCKY is a language and its compiler pipeline is the product. This project implements a stage-based pipeline where every transformation is an isolated, testable stage. It uses a profile-driven approach to allow users to select obfuscation levels seamlessly.

## Setup

```bash
python3 -m venv venv
source venv/bin/activate
pip install -e .
```

## Usage

```bash
jocky build main.jky --profile standard
jocky run main.jky
jocky clean
jocky info main.jky
jocky verify main.jky
```

## Structure
See `docs/architecture.md` for more information on the structure.
