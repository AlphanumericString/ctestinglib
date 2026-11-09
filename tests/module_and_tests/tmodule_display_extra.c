#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int display_empty(void)
{
	t_module *empty;

	ct_module_init(&empty, "empty", "no tests");
	ct_module_display(empty, CT_DM_SUMPCT);
	ct_module_display(empty, CT_DM_SUMPCT | CT_FMT_JSN);
	ct_module_display(empty, CT_DM_SUMNBS);
	ct_module_display(empty, CT_DISPLAY_DEFAULT);
	ct_module_destroy(empty);
	return (EXIT_SUCCESS);
}

static int display_results(void)
{
	t_module *fail;

	ct_module_init(&fail, "fail", "failing results");
	ct_module_taddf(fail, dummy_fail_test, "ko");
	ct_module_taddf(fail, dummy_sigsegv, "crash");
	ct_module_run(fail);
	ct_module_display(fail, CT_DT_OK);
	ct_module_display(fail, CT_FMT_JSN);
	ct_module_display(fail, CT_FMT_JSN | CT_DT_OK);
	ct_module_destroy(fail);
	return (EXIT_SUCCESS);
}

static int display_submodules(void)
{
	t_module *root;
	t_module *sub1;
	t_module *sub2;

	ct_module_init(&root, "root", "root description");
	sub1 = create_quiet_dummy();
	sub2 = create_quiet_dummy();
	if (!sub1 || !sub2)
		return (ct_module_destroy(root), 1);
	ct_module_add_sub(root, sub1);
	ct_module_add_sub(root, sub2);
	ct_module_taddf(root, dummy_success_test, "r1");
	ct_module_run(root);
	ct_module_display(root, CT_DISPLAY_DEFAULT);
	ct_module_display(root, CT_DSBM_SEP);
	ct_module_display(root, CT_DSBM_SEP | CT_DM_SUMNBS);
	ct_module_display(root, CT_DSBM_SEP | CT_DSBM_SUMPCT);
	ct_module_display(root, CT_DSBM_SUMPCT);
	ct_module_display(root, CT_DM_SUMSILENT);
	ct_module_display(root, CT_FMT_JSN);
	ct_module_display(root, CT_FMT_JSN | CT_DSBM_SEP);
	ct_module_destroy(root);
	ct_module_init(&root, "parent", "no direct tests");
	sub1 = create_quiet_dummy();
	if (!sub1)
		return (ct_module_destroy(root), 2);
	ct_module_add_sub(root, sub1);
	ct_module_run(root);
	ct_module_display(root, CT_DSBM_SEP | CT_DM_SUMNBS);
	return (ct_module_destroy(root), EXIT_SUCCESS);
}

static int display_internal(void)
{
	t_module *root;
	t_module *sub;

	ct_module_init(&root, "root", "root description");
	sub = create_quiet_dummy();
	if (!sub)
		return (ct_module_destroy(root), 1);
	ct_module_add_sub(root, sub);
	cti_display_results_json(root, CT_FMT_JSN | CT_DM_SUMSILENT);
	cti_mdisplay_sum_nb(1, 1, CT_FMT_SILENT);
	cti_mdisplay_sum_percent(1, 1, CT_FMT_SILENT);
	cti_mdisplay_sbm(root->submodules_list, CT_FMT_JSN);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

static int display_guards(void)
{
	t_module *root;

	root = create_quiet_dummy();
	ct_module_display(NULL, CT_DISPLAY_DEFAULT);
	ct_module_display(root, CT_FMT_SILENT);
	ct_module_display(root, CT_FMT_JSN | CT_DM_SUMSILENT);
	ct_module_display(root, (t_display_mode)(2 << 6));
	ct_module_display(root, CT_DM_SUMPCT);
	ct_module_display(root, CT_DM_SUMPCT | CT_PRTY_NOCOLOR);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

int ctit_module_display_extra(void)
{
	int ret;

	ret = display_empty();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_results();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_submodules();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_internal();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_guards();
	return (ret);
}
