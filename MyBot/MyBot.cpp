#include "MyBot.h"
#include <dpp/dpp.h>

// FIXME: 
//	�� ��� ����¿��� �ѱ��� �������� ����...
// UTF-8 �����ϱ�?

/* ��ū�� �ݵ�� key ���Ͽ� ���� �� ������ּ���!
 * ����� �߱޹޴� ���!
 * https://dpp.dev/creating-a-bot-application.html
 * ���� �ʴ��� ���� �ݵ�� '��' ������
 * ������ �����Ͽ� �ʴ��Ͻʽÿ�. Ex. 'applications.commands'
 */

int main()
{
	const std::string BOT_TOKEN = get_discord_token();

	/* �� Ŭ������ ����*/
	dpp::cluster bot(BOT_TOKEN);

	/* ǥ����¿� ������ �α� �޽��� ��� */
	bot.on_log(dpp::utility::cout_logger());

	/* on_ready�� ������ ��ɾ� ��� */
	bot.on_ready([&bot](const dpp::ready_t& event)
	{
		/* run_once�� ���Ͽ� �翬�� ������ ������� �ʵ��� ��.*/
		if (dpp::run_once<struct register_bot_commands>())
		{
			std::vector<dpp::slashcommand> commands
			{
				{ "ping", "We play \"Ping pong!\" for a test!", bot.me.id }
			};
			bot.global_bulk_command_create(commands);
		}
	});

	/* D++ ���, �ڷ�ƾ�� ���� �ֱٿ� �߰��� ������� ������ ����� ó�� */
	bot.on_slashcommand([](const dpp::slashcommand_t& event) -> dpp::task<void>
	{
			if (event.command.get_command_name() == "ping")
				co_await event.co_reply("!P!O!N!G!");
			co_return;
	});

	/* �� ���� */
	bot.start(dpp::st_wait);

	return 0;
}
